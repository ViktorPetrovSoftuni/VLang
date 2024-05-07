export const metadata = {
  title: 'Home - Simple',
  description: 'Page description',
}

import Hero from '@/components/hero'
import Features from '@/components/features'
import FeaturesBlocks from '@/components/features-blocks'

export default function Home() {
  return (
    <>
      <Hero />
      <Features />
      <FeaturesBlocks />
    </>
  )
}
